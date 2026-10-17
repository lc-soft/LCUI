import { RadioGroup, RadioGroupItem } from "@lcui/react";
import { Label } from "@lcui/react";

export default function App() {
  return (
    <RadioGroup value="option2" className="radio-group-demo">
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="disabled" id="rd1" disabled />
        <Label for="rd1">Disabled</Label>
      </div>
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="option2" id="rd2" checked />
        <Label for="rd2">Option 2</Label>
      </div>
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="option3" id="rd3" />
        <Label for="rd3">Option 3</Label>
      </div>
    </RadioGroup>
  );
}


