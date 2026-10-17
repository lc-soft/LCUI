import { Checkbox, Label, Widget } from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2">
      <Widget className="flex items-center gap-2">
        <Checkbox id="select-all" indeterminate />
        <Label for="select-all">Select all</Label>
      </Widget>
      <Widget className="flex items-center gap-2">
        <Checkbox id="option-a" checked />
        <Label for="option-a">Option A</Label>
      </Widget>
      <Widget className="flex items-center gap-2">
        <Checkbox id="option-b" />
        <Label for="option-b">Option B</Label>
      </Widget>
    </Widget>
  );
}
