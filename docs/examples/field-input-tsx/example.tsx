import {
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldSet,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldSet>
        <FieldGroup>
          <Field>
            <FieldLabel htmlFor="username">Username</FieldLabel>
            <TextInput id="username" placeholder="Max Leiter" />
            <FieldDescription>Choose a unique username for your account.</FieldDescription>
          </Field>
          <Field>
            <FieldLabel htmlFor="password">Password</FieldLabel>
            <FieldDescription>Must be at least 8 characters long.</FieldDescription>
            <TextInput id="password" placeholder="••••••••" />
          </Field>
        </FieldGroup>
      </FieldSet>
    </Widget>
  );
}
