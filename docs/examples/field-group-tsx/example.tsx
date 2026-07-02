import {
  Checkbox,
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldSeparator,
  FieldSet,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldGroup>
        <FieldSet>
          <FieldLabel>Responses</FieldLabel>
          <FieldDescription>
            Get notified when ChatGPT responds to requests that take time,
            like research or image generation.
          </FieldDescription>
          <FieldGroup>
            <Field orientation="horizontal">
              <Checkbox id="push" defaultChecked disabled />
              <FieldLabel htmlFor="push">Push notifications</FieldLabel>
            </Field>
          </FieldGroup>
        </FieldSet>
        <FieldSeparator />
        <FieldSet>
          <FieldLabel>Tasks</FieldLabel>
          <FieldDescription>
            Get notified when tasks you&apos;ve created have updates.
          </FieldDescription>
          <FieldGroup>
            <Field orientation="horizontal">
              <Checkbox id="push-tasks" />
              <FieldLabel htmlFor="push-tasks">Push notifications</FieldLabel>
            </Field>
            <Field orientation="horizontal">
              <Checkbox id="email-tasks" />
              <FieldLabel htmlFor="email-tasks">Email notifications</FieldLabel>
            </Field>
          </FieldGroup>
        </FieldSet>
      </FieldGroup>
    </Widget>
  );
}
