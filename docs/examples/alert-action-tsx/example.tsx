import {
  Alert,
  AlertAction,
  AlertContent,
  AlertDescription,
  AlertTitle,
  Button,
} from "@lcui/react";

export default function Example() {
  return (
    <Alert className="w-full max-w-md">
      <AlertContent>
        <AlertTitle>Dark mode is now available</AlertTitle>
        <AlertDescription>
          Enable it under your profile settings to get started.
        </AlertDescription>
      </AlertContent>
      <AlertAction>
        <Button variant="primary" size="xs">
          Enable
        </Button>
      </AlertAction>
    </Alert>
  );
}
