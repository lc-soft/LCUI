import { Alert, AlertContent, AlertDescription, AlertTitle } from "@lcui/react";
import { CheckmarkCircle, Info } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <widget className="block w-full max-w-md">
      <Alert className="mb-6">
        <CheckmarkCircle />
        <AlertContent>
          <AlertTitle>Payment successful</AlertTitle>
          <AlertDescription>
            Your payment of $29.99 has been processed. A receipt has been sent
            to your email address.
          </AlertDescription>
        </AlertContent>
      </Alert>
      <Alert className="mb-6">
        <Info />
        <AlertContent>
          <AlertTitle>New feature available</AlertTitle>
          <AlertDescription>
            We&apos;ve added dark mode support. You can enable it in your
            account settings.
          </AlertDescription>
        </AlertContent>
      </Alert>
    </widget>
  );
}
