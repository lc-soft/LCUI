import { RadioGroup, RadioGroupItem } from "@lcui/react";
import { Label } from "@lcui/react";

export default function App() {
  return (
    <RadioGroup value="comfortable">
      <div className="flex items-center gap-3">
        <RadioGroupItem value="default" id="r1" />
        <Label for="r1">Default</Label>
      </div>
      <div className="flex items-center gap-3">
        <RadioGroupItem value="comfortable" id="r2" checked={true} />
        <Label for="r2">Comfortable</Label>
      </div>
      <div className="flex items-center gap-3">
        <RadioGroupItem value="compact" id="r3" />
        <Label for="r3">Compact</Label>
      </div>
    </RadioGroup>
  );
}