import { RadioGroup, RadioGroupItem } from "@lcui/react";
import { Label } from "@lcui/react";

export default function App() {
  return (
    <RadioGroup value="comfortable" className="radio-group-demo">
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="default" id="r1" />
        <Label for="r1">Default</Label>
      </div>
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="comfortable" id="r2" checked />
        <Label for="r2">Comfortable</Label>
      </div>
      <div className="flex flex-row items-center gap-3 flex-nowrap">
        <RadioGroupItem value="compact" id="r3" />
        <Label for="r3">Compact</Label>
      </div>
    </RadioGroup>
  );
}


