import { Shimmer, ShimmerContent, Text, Widget } from "@lcui/react";

export default function ShimmerDurationDemo() {
  return (
    <Widget className="flex flex-col gap-2">
      <Shimmer className="w-[220px] h-7" running>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Default duration</Text>
        </ShimmerContent>
      </Shimmer>
      <Shimmer className="w-[220px] h-7" running duration={1000}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Faster duration</Text>
        </ShimmerContent>
      </Shimmer>
    </Widget>
  );
}
