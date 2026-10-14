import { Spinner, SpinnerContent, Text } from "@lcui/react";

export default function SpinnerCustomContentDemo() {
  return (
    <div className="flex flex-row items-center gap-7">
      <Spinner className="w-10 h-10">
        <SpinnerContent className="relative flex items-center justify-center">
          <div className="absolute left-[15px] top-[3px] w-[10px] h-[10px] rounded-full bg-blue-500" />
          <div className="absolute left-[18px] top-[6px] w-1 h-5 rounded bg-red-500" />
        </SpinnerContent>
      </Spinner>
      <Spinner className="w-[96px] h-8">
        <SpinnerContent className="flex items-center justify-center">
          <Text className="whitespace-nowrap text-lg text-[#e5484d]">
            Hello
          </Text>
        </SpinnerContent>
      </Spinner>
    </div>
  );
}
