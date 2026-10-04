import { Alert, AlertContent, AlertDescription, AlertTitle } from "@lcui/react";
import { ErrorCircle } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Alert variant="destructive" className="w-full max-w-md">
      <ErrorCircle />
      <AlertContent>
        <AlertTitle>Payment failed</AlertTitle>
        <AlertDescription>
          Your payment could not be processed. Please check your payment method
          and try again.
        </AlertDescription>
      </AlertContent>
    </Alert>
  );
}
