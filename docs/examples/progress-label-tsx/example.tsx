import { Progress, ProgressLabel, ProgressValue } from "@lcui/react";

export default function App() {
  return (
    <Progress value={56} className="progress-label-demo">
      <ProgressLabel>Upload progress</ProgressLabel>
      <ProgressValue />
    </Progress>
  );
}
