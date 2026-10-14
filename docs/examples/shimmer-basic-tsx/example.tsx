import { Shimmer, ShimmerContent, Text } from "@lcui/react";

export default function ShimmerDemo() {
  return (
    <Shimmer className="w-[220px] h-7">
      <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
        <Text>Generating response…</Text>
      </ShimmerContent>
    </Shimmer>
  );
}
