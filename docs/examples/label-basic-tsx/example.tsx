import { Label, Checkbox } from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex gap-2">
      <Checkbox id="terms" />
      <Label for="terms">Accept terms and conditions</Label>
    </Widget>
  );
}