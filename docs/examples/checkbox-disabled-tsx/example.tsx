import { Checkbox } from "@lcui/react";
import { Label } from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2">
      <div className="flex items-center gap-2">
        <Checkbox id="d1" disabled={true} checked={true} />
        <Label for="d1">Disabled (pre-checked)</Label>
      </div>
      <div className="flex items-center gap-2">
        <Checkbox id="d2" checked={true} />
        <Label for="d2">Normal</Label>
      </div>
    </Widget>
  );
}