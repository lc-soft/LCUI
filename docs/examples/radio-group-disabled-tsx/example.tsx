import { RadioGroup, RadioGroupItem } from "@lcui/react";
import { Label } from "@lcui/react";

export default function App() {
  return (
    <RadioGroup value="option2">
      <div className="flex items-center gap-3">
        <RadioGroupItem value="disabled" id="rd1" disabled={true} />
        <Label for="rd1">Disabled</Label>
      </div>
      <div className="flex items-center gap-3">
        <RadioGroupItem value="option2" id="rd2" checked={true} />
        <Label for="rd2">Option 2</Label>
      </div>
      <div className="flex items-center gap-3">
        <RadioGroupItem value="option3" id="rd3" />
        <Label for="rd3">Option 3</Label>
      </div>
    </RadioGroup>
  );
}