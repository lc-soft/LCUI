import { Shimmer, ShimmerContent, Text, Widget } from "@lcui/react";

export default function ShimmerSpreadDemo() {
  return (
    <Widget className="flex flex-col gap-2">
      <Shimmer className="w-[220px] h-7" running spread={40}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Narrow spread</Text>
        </ShimmerContent>
      </Shimmer>
      <Shimmer className="w-[220px] h-7" running spread={160}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Wide spread</Text>
        </ShimmerContent>
      </Shimmer>
    </Widget>
  );
}
