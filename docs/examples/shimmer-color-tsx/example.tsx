import { Shimmer, ShimmerContent, Text, Widget } from "@lcui/react";

export default function ShimmerColorDemo() {
  return (
    <Widget className="flex flex-col gap-2">
      <Shimmer className="w-[220px] h-7" color="#378ADD">
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Generating response…</Text>
        </ShimmerContent>
      </Shimmer>
      <Shimmer className="w-[220px] h-7" color="rgb(34, 197, 94)">
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Sending request…</Text>
        </ShimmerContent>
      </Shimmer>
    </Widget>
  );
}
