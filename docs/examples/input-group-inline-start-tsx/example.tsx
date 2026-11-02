import {
  Field,
  FieldDescription,
  FieldLabel,
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
} from "@lcui/react";

export default function Example() {
  return (
    <Field className="w-[384px]">
      <FieldLabel htmlFor="inline-start-input">Input</FieldLabel>
      <InputGroup>
        <InputGroupInput id="inline-start-input" placeholder="Search..." />
        <InputGroupAddon align="inline-start">
          <Icon name="search" />
        </InputGroupAddon>
      </InputGroup>
      <FieldDescription>Icon positioned at the start.</FieldDescription>
    </Field>
  );
}
