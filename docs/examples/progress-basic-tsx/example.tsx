import { Progress, useState, CType } from "@lcui/react";

export default function ProgressDemo() {
  const [value] = useState(20, CType.Int);
  return <Progress value={value} />;
}
