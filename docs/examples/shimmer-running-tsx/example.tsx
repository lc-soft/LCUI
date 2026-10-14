import { Shimmer, ShimmerContent, Text, Widget } from "@lcui/react";

export default function ShimmerRunningDemo() {
  return (
    <Widget className="flex flex-col gap-2">
      <Shimmer className="w-[220px] h-7" running>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Searching the web…</Text>
        </ShimmerContent>
      </Shimmer>
      <Shimmer className="w-[220px] h-7" running={false}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Reading 4 files</Text>
        </ShimmerContent>
      </Shimmer>
    </Widget>
  );
}
