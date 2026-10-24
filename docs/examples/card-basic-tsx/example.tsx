import {
  Button,
  Card,
  CardAction,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
  Field,
  FieldLabel,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Card className="w-full max-w-sm">
      <CardHeader>
        <CardTitle>Login to your account</CardTitle>
        <CardDescription>
          Enter your email below to login to your account
        </CardDescription>
        <CardAction>
          <Button variant="link">Sign Up</Button>
        </CardAction>
      </CardHeader>
      <CardContent className="clip-safe">
        <Widget className="clip-safe flex flex-col gap-6">
          <Field className="clip-safe">
            <FieldLabel htmlFor="card-email">Email</FieldLabel>
            <TextInput
              className="w-full"
              id="card-email"
              type="email"
              placeholder="m@example.com"
            />
          </Field>
          <Field className="clip-safe">
            <Widget className="flex items-center">
              <FieldLabel htmlFor="card-password">Password</FieldLabel>
              <Button
                className="ml-auto text-sm text-foreground"
                variant="link"
                size="sm"
              >
                Forgot your password?
              </Button>
            </Widget>
            <TextInput className="w-full" id="card-password" type="password" />
          </Field>
        </Widget>
      </CardContent>
      <CardFooter className="flex-col gap-2">
        <Button className="w-full" variant="primary">
          Login
        </Button>
        <Button className="w-full" variant="outline">
          Login with Google
        </Button>
      </CardFooter>
    </Card>
  );
}
