import { Spinner, Widget } from "@lcui/react";

export default function SpinnerSizeDemo() {
  return (
    <Widget className="flex items-center gap-4">
      <Spinner className="w-6 h-6" />
      <Spinner className="w-10 h-10" />
      <Spinner className="w-16 h-16" />
    </Widget>
  );
}
