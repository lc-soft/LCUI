import { Spinner, Widget } from "@lcui/react";

export default function SpinnerRunningDemo() {
  return (
    <Widget className="flex items-center gap-6">
      <Spinner className="w-10 h-10" running speed={0.5} />
      <Spinner className="w-10 h-10" running={false} angle={90} />
    </Widget>
  );
}
