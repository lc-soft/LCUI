const test = require("node:test");
const assert = require("node:assert");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { fetchRepoStars } = require("./fetch-repo-stars");

const DAY = 24 * 60 * 60 * 1000;

function tempPaths() {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "repo-stars-"));
  return {
    dir,
    cachePath: path.join(dir, "repo-stars.json"),
    outPath: path.join(dir, "repo-stars.generated.ts"),
  };
}

function fakeFetch(counts) {
  return async (url) => {
    const key = url.includes("github.com") ? "github" : "gitee";
    if (counts[key] === "fail") {
      return { ok: false, status: 500, json: async () => ({}) };
    }
    return { ok: true, json: async () => ({ stargazers_count: counts[key] }) };
  };
}

test("uses a fresh cache without requesting", async () => {
  const p = tempPaths();
  fs.writeFileSync(
    p.cachePath,
    JSON.stringify({
      github: { count: 10, fetchedAt: 1000 },
      gitee: { count: 20, fetchedAt: 1000 },
    })
  );
  let calls = 0;
  const result = await fetchRepoStars({
    ...p,
    now: () => 2000,
    maxAgeMs: DAY,
    fetchImpl: async () => {
      calls++;
      return { ok: false, json: async () => ({}) };
    },
  });
  assert.equal(calls, 0);
  assert.equal(result.github.count, 10);
  assert.equal(result.gitee.count, 20);
});

test("refreshes expired entries and writes the generated file", async () => {
  const p = tempPaths();
  fs.writeFileSync(
    p.cachePath,
    JSON.stringify({
      github: { count: 10, fetchedAt: 0 },
      gitee: { count: 20, fetchedAt: 0 },
    })
  );
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: fakeFetch({ github: 111, gitee: 222 }),
  });
  assert.equal(result.github.count, 111);
  assert.equal(result.gitee.count, 222);
  assert.match(fs.readFileSync(p.outPath, "utf8"), /github: 111/);
});

test("keeps the old value when one source fails", async () => {
  const p = tempPaths();
  fs.writeFileSync(
    p.cachePath,
    JSON.stringify({
      github: { count: 10, fetchedAt: 0 },
      gitee: { count: 20, fetchedAt: 0 },
    })
  );
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: fakeFetch({ github: 333, gitee: "fail" }),
  });
  assert.equal(result.github.count, 333);
  assert.equal(result.gitee.count, 20);
});

test("writes null when there is no cache and the request fails", async () => {
  const p = tempPaths();
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: fakeFetch({ github: "fail", gitee: "fail" }),
  });
  assert.equal(result.github, null);
  assert.equal(result.gitee, null);
  assert.match(fs.readFileSync(p.outPath, "utf8"), /github: null/);
});

test("recovers from a corrupt cache file", async () => {
  const p = tempPaths();
  fs.writeFileSync(p.cachePath, "{ not json");
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: fakeFetch({ github: 42, gitee: 43 }),
  });
  assert.equal(result.github.count, 42);
  assert.equal(result.gitee.count, 43);
});

test("keeps a fresh value when the other source rejects", async () => {
  const p = tempPaths();
  fs.writeFileSync(
    p.cachePath,
    JSON.stringify({
      github: { count: 10, fetchedAt: 9 * DAY },
      gitee: { count: 20, fetchedAt: 0 },
    })
  );
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: async () => {
      throw new Error("network down");
    },
  });
  assert.equal(result.github.count, 10);
  assert.equal(result.gitee.count, 20);
});

test("writes null when a request rejects and there is no cache", async () => {
  const p = tempPaths();
  const result = await fetchRepoStars({
    ...p,
    now: () => 10 * DAY,
    maxAgeMs: DAY,
    fetchImpl: async () => {
      throw new Error("network down");
    },
  });
  assert.equal(result.github, null);
  assert.equal(result.gitee, null);
  assert.match(fs.readFileSync(p.outPath, "utf8"), /gitee: null/);
});
