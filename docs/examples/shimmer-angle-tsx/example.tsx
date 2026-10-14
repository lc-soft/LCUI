import { Shimmer, ShimmerContent, Text, Widget } from "@lcui/react";

export default function ShimmerAngleDemo() {
  return (
    <Widget className="flex flex-col gap-2">
      <Shimmer className="w-[220px] h-9" running angle={0}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Flat band</Text>
        </ShimmerContent>
      </Shimmer>
      <Shimmer className="w-[220px] h-9" running angle={45}>
        <ShimmerContent className="flex items-center px-2 py-1 text-[#46545f]">
          <Text>Tilted band</Text>
        </ShimmerContent>
      </Shimmer>
    </Widget>
  );
}
