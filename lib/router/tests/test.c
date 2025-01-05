/*
 * lib/router/tests/test.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../src/private.h"
#include <ctest.h>
#include <router.h>

void test_router_location(void)
{
        router_route_t *route;
        router_location_t *raw;
        router_location_t *location;
        router_route_record_t *record;
        const char *str;
        char *path;

        raw = router_location_create(NULL, "/search?type=issue&order=desc");
        location = router_location_normalize(raw, NULL, false);

        str = strmap_get(location->query, "type");
        ctest_equal_str("location.query.type", str, "issue");

        str = strmap_get(location->query, "order");
        ctest_equal_str("location.query.order", str, "desc");

        str = location->path;
        ctest_equal_str("location.path", str, "/search");

        router_location_destroy(raw);
        router_location_destroy(location);

        raw = router_location_create(NULL, "/search?type=issue#pagination");
        location = router_location_normalize(raw, NULL, false);

        str = location->hash;
        ctest_equal_str("location.hash", str, "#pagination");

        router_location_destroy(raw);
        router_location_destroy(location);

        record = router_route_record_create();
        location = router_location_create(NULL, "/repos/root/example/issues");
        router_route_record_set_path(record, "/repos/:user/:repo/issues");
        route = router_route_create(record, location);
        router_location_destroy(location);
        raw = router_location_create(NULL, NULL);
        router_location_set_param(raw, "user", "foo");
        router_location_set_param(raw, "repo", "bar");
        location = router_location_normalize(raw, route, false);
        ctest_equal_str(
            "normalize({ params: { user: 'foo', repo: 'bar' } }).path",
            router_location_get_path(location), "/repos/foo/bar/issues");
        router_location_destroy(raw);
        router_location_destroy(location);

        raw = router_location_create(NULL, NULL);
        router_location_set_param(raw, "user", "root");
        router_location_set_param(raw, "repo", "example");
        location = router_location_normalize(raw, route, false);
        ctest_equal_str(
            "normalize({ params: { user: 'root', repo: 'example' } }).path",
            router_location_get_path(location), "/repos/root/example/issues");
        router_location_destroy(raw);
        router_location_destroy(location);

        raw = router_location_create(NULL, NULL);
        router_location_set_query(raw, "q", "bug");
        router_location_set_query(raw, "state", "closed");
        location = router_location_normalize(raw, route, false);
        path = router_location_stringify(location);
        ctest_equal_str(
            "stringify(normalize({ query: { q: 'bug', state: 'closed' } }))",
            path, "/repos/root/example/issues?q=bug&state=closed");
        free(path);
        router_location_destroy(raw);
        router_location_destroy(location);

        raw = router_location_create(NULL, NULL);
        router_location_set_query(raw, "order", "desc");
        router_location_set_query(raw, "assignee", "root");
        location = router_location_normalize(raw, route, false);
        path = router_location_stringify(location);
        ctest_equal_str("stringify(normalize({ query: { order: 'desc', "
                        "assignee: 'root' } }))",
                        path,
                        "/repos/root/example/issues?order=desc&assignee=root");
        free(path);
        router_location_destroy(raw);
        router_location_destroy(location);

        router_route_record_destroy(record);
        router_route_destroy(route);
}

void test_router_route(void)
{
        router_route_t *route;
        router_location_t *location;
        router_location_t *raw;
        const char *full_path =
            "/user/profile?tab=repos&order=desc&q=test#pagination";

        raw = router_location_create(NULL, full_path);
        location = router_location_normalize(raw, NULL, false);
        route = router_route_create(NULL, location);

        ctest_equal_str("route.path", router_route_get_path(route),
                        "/user/profile");
        ctest_equal_str("route.query.q", router_route_get_query(route, "q"),
                        "test");
        ctest_equal_str("route.query.tab", router_route_get_query(route, "tab"),
                        "repos");
        ctest_equal_str("route.query.order",
                        router_route_get_query(route, "order"), "desc");
        ctest_equal_str("route.query.other",
                        router_route_get_query(route, "other"), NULL);
        ctest_equal_str("route.hash", router_route_get_hash(route),
                        "#pagination");
        ctest_equal_str("route.fullPath.length",
                        router_route_get_full_path(route), full_path);
        router_location_destroy(raw);
        router_location_destroy(location);
        router_route_destroy(route);
}

void test_router_matcher(void)
{
        router_t *router;
        router_config_t *config;
        router_route_t *route;
        router_route_record_t *route_user_show;
        router_resolved_t *resolved;
        router_location_t *location;
        const router_route_record_t *record;
        const char *str;

        router = router_create(NULL);
        config = router_config_create();
        router_config_set_path(config, "/home");
        router_config_set_component(config, NULL, "home");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/files");
        router_config_set_component(config, NULL, "files");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/files/*");
        router_config_set_component(config, NULL, "files");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/users");
        router_config_set_component(config, NULL, "user-index");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/users/:username");
        router_config_set_component(config, NULL, "user-show");
        route_user_show = router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "");
        router_config_set_component(config, NULL, "user-overview");
        router_add_route_record(router, config, route_user_show);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "profile");
        router_config_set_component(config, NULL, "user-profile");
        router_add_route_record(router, config, route_user_show);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "posts");
        router_config_set_name(config, "user#posts");
        router_config_set_component(config, NULL, "user-posts");
        router_add_route_record(router, config, route_user_show);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/about");
        router_config_set_component(config, NULL, "about");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "*");
        router_config_set_component(config, NULL, "not-found");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        location = router_location_create(NULL, "/users/root");
        resolved = router_resolve(router, location, false);
        router_location_destroy(location);
        route = router_resolved_get_route(resolved);
        str = router_route_get_param(route, "username");
        ctest_equal_str("match('/users/root').route.params.username", str,
                        "root");
        router_resolved_destroy(resolved);

        location = router_location_create("user#posts", NULL);
        resolved = router_resolve(router, location, false);
        ctest_equal_bool("match({ name: 'user#posts' })", !resolved, false);
        router_location_destroy(location);
        router_resolved_destroy(resolved);

        location = router_location_create(NULL, "/users/root");
        router_push(router, location);
        router_location_destroy(location);
        location = router_location_create("user#posts", NULL);
        resolved = router_resolve(router, location, false);
        ctest_equal_bool("match({ name: 'user#posts' })", !!resolved, true);
        route = router_resolved_get_route(resolved);
        str = router_route_get_param(route, "username");
        ctest_equal_str("match({ name: 'user#posts' }).route.params.username",
                        router_route_get_param(route, "username"), "root");
        ctest_equal_str("match({ name: 'user#posts' }).route.fullPath",
                        router_route_get_full_path(route), "/users/root/posts");
        router_location_destroy(location);
        router_resolved_destroy(resolved);

        location = router_location_create(NULL, "/users/root/posts");
        resolved = router_resolve(router, location, false);
        router_location_destroy(location);
        route = router_resolved_get_route(resolved);
        record = router_route_get_matched_record(route, 0);
        ctest_equal_bool("match('/users/root/posts').route.matched[0] != null",
                         !!record, true);
        ctest_equal_str(
            "match('/users/root/posts').route.matched[0].components.default",
            record ? router_route_record_get_component(record, NULL) : NULL,
            "user-show");
        route = router_resolved_get_route(resolved);
        record = router_route_get_matched_record(route, 1);
        ctest_equal_bool("match('/users/root/posts').route.matched[1] != null",
                         !!record, true);
        ctest_equal_str(
            "match('/users/root/posts').route.matched[1].components.default",
            record ? router_route_record_get_component(record, NULL) : NULL,
            "user-posts");
        router_resolved_destroy(resolved);

        location = router_location_create(NULL, "/other/path/to/file");
        resolved = router_resolve(router, location, false);
        router_location_destroy(location);
        route = router_resolved_get_route(resolved);
        record = router_route_get_matched_record(route, 0);
        ctest_equal_bool(
            "match('/other/path/to/file').route.matched[0] != null", !!record,
            true);
        ctest_equal_str(
            "match('/other/path/to/file').route.matched[0].components.default",
            record ? router_route_record_get_component(record, NULL) : NULL,
            "not-found");
        ctest_equal_str(
            "match('/other/path/to/file').route.params.pathMatch",
            route ? router_route_get_param(route, "pathMatch") : NULL,
            "other/path/to/file");
        router_resolved_destroy(resolved);

        location = router_location_create(NULL, "/files/path/to/file");
        resolved = router_resolve(router, location, false);
        router_location_destroy(location);
        route = router_resolved_get_route(resolved);
        record = router_route_get_matched_record(route, 0);
        ctest_equal_str(
            "match('/files/path/to/file').route.matched[0].components.default",
            record ? router_route_record_get_component(record, NULL) : NULL,
            "files");
        ctest_equal_str(
            "match('/files/path/to/file').route.params.pathMatch",
            route ? router_route_get_param(route, "pathMatch") : NULL,
            "path/to/file");
        router_resolved_destroy(resolved);

        router_destroy(router);
}

void test_router_utils(void)
{
        char *str;
        const char *p;
        char key[256];
        size_t key_len;
        strmap_t *a;
        strmap_t *b;
        strmap_t *params;

        a = strmap_create();
        b = strmap_create();
        strmap_set(a, "id", "404");
        strmap_set(b, "id", "404");
        strmap_set(a, "name", "git");
        strmap_set(b, "name", "git");

        ctest_equal_bool(
            "isObjectEqual({ id: '404', name: 'git' }, { id: '404', name: "
            "'git' })",
            strmap_equal(a, b), true);

        strmap_set(b, "id", "200");
        ctest_equal_bool(
            "isObjectEqual({ id: '404', name: 'git' }, { id: '200', name: "
            "'git' })",
            strmap_equal(a, b), false);

        strmap_delete(b, "id");
        ctest_equal_bool(
            "isObjectIncludes({ id: '404', name: 'git' }, { name: 'git' })",
            strmap_includes(a, b), true);
        strmap_delete(a, "name");
        ctest_equal_bool("isObjectIncludes({ id: '404' }, { name: '200' })",
                         strmap_includes(a, b), false);
        strmap_delete(a, "id");
        strmap_delete(b, "name");
        ctest_equal_bool("isObjectEqual({}, {})", strmap_equal(a, b), true);
        ctest_equal_int("string.compare('', '')", router_string_compare("", ""),
                        0);
        ctest_equal_bool("string.compare(null, 'a') != 0",
                         router_string_compare(NULL, "a") != 0, true);
        ctest_equal_int("string.compare(null, null)",
                        router_string_compare(NULL, NULL), 0);
        strmap_destroy(a);
        strmap_destroy(b);

        str = router_path_resolve("", NULL, true);
        ctest_equal_str("path.resolve('', null, true) == '/'", str, "/");
        free(str);

        str = router_path_resolve("", "/root", true);
        ctest_equal_str("path.resolve('', '/root', true)", str, "/root");
        free(str);

        str = router_path_resolve("hello/../world/./", NULL, true);
        ctest_equal_str("path.resolve('hello/../world/./', null, true)", str,
                        "/world");
        free(str);

        str = router_path_resolve("/root/path", "base/path", true);
        ctest_equal_str("path.resolve('/root/path', 'base/path', true)", str,
                        "/root/path");
        free(str);

        str = router_path_resolve("../../profile", "base/path/to/file", true);
        ctest_equal_str(
            "path.resolve('../../profile', 'base/path/to/file', true)", str,
            "/base/path/profile");
        free(str);

        str = router_path_resolve("profile", "base/file", false);
        ctest_equal_str("path.resolve('profile', 'base/profile', false)", str,
                        "/base/profile");
        free(str);

        str = router_path_resolve("/profile", "base/file", false);
        ctest_equal_str("path.resolve('/profile', 'base/profile', false)", str,
                        "/profile");
        free(str);

        ctest_equal_bool("path.compare('/a/b', '/a/b/')",
                         router_path_compare("/a/b", "/a/b/"), true);
        ctest_equal_bool("path.compare('/a/b/', '/a/b/')",
                         router_path_compare("/a/b/", "/a/b"), true);
        ctest_equal_bool("path.compare('', '')",
                         router_path_compare("", "") == 0, true);
        ctest_equal_bool("path.compare('a', '')",
                         router_path_compare("a", "") != 0, true);
        ctest_equal_bool("path.compare('', 'b')",
                         router_path_compare("", "b") != 0, true);
        ctest_equal_bool("path.compare('a', 'b')",
                         router_path_compare("a", "b") != 0, true);

        ctest_equal_bool(
            "path.startsWith('/profile/events', '/profile/event')",
            router_path_starts_with("/profile/events", "/profile/event"),
            false);
        ctest_equal_bool(
            "path.startsWith('/profile/events', '/profile/')",
            router_path_starts_with("/profile/events", "/profile/"), true);
        ctest_equal_bool("path.startsWith('/profile', '/profile/')",
                         router_path_starts_with("/profile", "/profile/"),
                         true);
        ctest_equal_bool("path.startsWith('/profile/', '/profile')",
                         router_path_starts_with("/profile/", "/profile"),
                         true);

        p = str = "/:username/:repo/settings";
        p = router_path_parse_key(p, key, &key_len);
        ctest_equal_str("path.keys('/:username/:repo/settings')[0].key", key,
                        "username");
        ctest_equal_int("path.keys('/:username/:repo/settings')[0].index",
                        (int)(p ? p - str : 0), 11);
        p = router_path_parse_key(p, key, &key_len);
        ctest_equal_str("path.keys('/:username/:repo/settings')[1].key", key,
                        "repo");
        ctest_equal_int("path.keys('/:username/:repo/settings')[1].index",
                        (int)(p ? p - str : 0), 17);
        p = router_path_parse_key(p, key, &key_len);
        ctest_equal_str("path.keys('/:username/:repo/settings')[2].key", key,
                        "");
        ctest_equal_int("path.keys('/:username/:repo/settings')[2].index",
                        (int)(p ? p - str : -1), -1);

        params = strmap_create();
        strmap_set(params, "1", "one");
        strmap_set(params, "2", "two");
        strmap_set(params, "3", "three");
        strmap_set(params, "word", "good");
        str = router_path_fill_params("/:1/:2/:3/foo", params);
        ctest_equal_str(
            "path.fillParams('/:1/:2/:3/foo', { 1: 'one', 2: 'two', 3: "
            "'three' })",
            str, "/one/two/three/foo");
        free(str);

        str = router_path_fill_params("/:1:2:3/foo", params);
        ctest_equal_str(
            "path.fillParams('/:1:2:3/foo', { 1: 'one', 2: 'two', 3: "
            "'three' })",
            str, NULL);

        strmap_destroy(params);
        str = router_path_fill_params("/foo/bar", NULL);
        ctest_equal_str("path.fillParams('/foo/bar')", str, "/foo/bar");
        free(str);
}

void test_router_history(void)
{
        router_t *router;
        router_config_t *config;
        router_location_t *location;
        router_history_t *history;
        const router_route_t *route;

        router = router_create(NULL);
        config = router_config_create();
        router_config_set_path(config, "/foo");
        router_config_set_component(config, NULL, "foo");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/foo/bar");
        router_config_set_component(config, NULL, "foobar");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/bar");
        router_config_set_component(config, NULL, "bar");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        history = router_get_history(router);
        location = router_location_create(NULL, "/foo/bar");
        router_push(router, location);
        route = router_get_current_route(router);
        ctest_equal_str("router.push('/foo/bar'), router.currentRoute.path",
                        router_route_get_path(route), "/foo/bar");
        router_location_destroy(location);

        location = router_location_create(NULL, "/bar");
        router_push(router, location);
        route = router_get_current_route(router);
        ctest_equal_str("router.push('/bar'), router.currentRoute.path",
                        router_route_get_path(route), "/bar");
        router_location_destroy(location);

        location = router_location_create(NULL, "/foo");
        router_push(router, location);
        route = router_get_current_route(router);
        ctest_equal_str("router.push('/foo'), router.currentRoute.path",
                        router_route_get_path(route), "/foo");
        router_location_destroy(location);

        ctest_equal_int("router.history.index",
                        (int)router_history_get_index(history), 2);

        router_back(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.back(), router.currentRoute.path",
                        router_route_get_path(route), "/bar");

        router_back(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.back(), router.currentRoute.path",
                        router_route_get_path(route), "/foo/bar");

        router_back(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.back(), router.currentRoute.path",
                        router_route_get_path(route), "/foo/bar");

        router_forward(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.forward(), router.currentRoute.path",
                        router_route_get_path(route), "/bar");

        router_back(router);
        router_go(router, 100);
        route = router_get_current_route(router);
        ctest_equal_str(
            "router.back(), router.go(100), router.currentRoute.path",
            router_route_get_path(route), "/foo");

        router_go(router, -2);
        route = router_get_current_route(router);
        ctest_equal_str(
            "router.back(), router.go(-2), router.currentRoute.path",
            router_route_get_path(route), "/foo/bar");

        router_go(router, -100);
        route = router_get_current_route(router);
        ctest_equal_str(
            "router.back(), router.go(-100), router.currentRoute.path",
            router_route_get_path(route), "/foo/bar");

        location = router_location_create(NULL, "/bar");
        ctest_equal_int("router.history.length",
                        (int)router_history_get_length(history), 3);
        router_push(router, location);
        ctest_equal_int("router.push('/bar'), router.history.length",
                        (int)router_history_get_length(history), 2);
        router_location_destroy(location);

        location = router_location_create(NULL, "/foo");
        router_replace(router, location);
        route = router_get_current_route(router);
        ctest_equal_str("router.replace('/foo'), router.currentRoute.path",
                        router_route_get_path(route), "/foo");
        router_location_destroy(location);

        router_back(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.back(), router.currentRoute.path",
                        router_route_get_path(route), "/foo/bar");

        router_forward(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.forward(), router.currentRoute.path",
                        router_route_get_path(route), "/foo");

        router_forward(router);
        route = router_get_current_route(router);
        ctest_equal_str("router.forward(), router.currentRoute.path",
                        router_route_get_path(route), "/foo");

        router_destroy(router);
}

int main(void)
{
        logger_set_level(LOGGER_LEVEL_OFF);
        ctest_describe("router utils", test_router_utils);
        ctest_describe("router location", test_router_location);
        ctest_describe("router route", test_router_route);
        ctest_describe("router matcher", test_router_matcher);
        ctest_describe("router history", test_router_history);
        return ctest_finish();
}
