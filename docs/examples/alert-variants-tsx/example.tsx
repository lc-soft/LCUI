import { Alert, AlertContent, AlertDescription, AlertTitle } from "@lcui/react";
import {
  CheckmarkCircle,
  ErrorCircle,
  Info,
  Warning,
} from "@lcui/fluent-icons";

export default function Example() {
  return (
    <widget className="block w-full max-w-md">
      <Alert variant="info" className="mb-6">
        <Info />
        <AlertContent>
          <AlertTitle>New feature available</AlertTitle>
          <AlertDescription>Try the updated dashboard.</AlertDescription>
        </AlertContent>
      </Alert>
      <Alert variant="success" className="mb-6">
        <CheckmarkCircle />
        <AlertContent>
          <AlertTitle>Payment successful</AlertTitle>
          <AlertDescription>Your payment has been processed.</AlertDescription>
        </AlertContent>
      </Alert>
      <Alert variant="warning" className="mb-6">
        <Warning />
        <AlertContent>
          <AlertTitle>Storage almost full</AlertTitle>
          <AlertDescription>Free up space to continue.</AlertDescription>
        </AlertContent>
      </Alert>
      <Alert variant="destructive" className="mb-6">
        <ErrorCircle />
        <AlertContent>
          <AlertTitle>Unable to save changes</AlertTitle>
          <AlertDescription>Please try again.</AlertDescription>
        </AlertContent>
      </Alert>
    </widget>
  );
}
