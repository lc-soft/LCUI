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
      <FieldLabel htmlFor="inline-end-input">Input</FieldLabel>
      <InputGroup>
        <InputGroupInput
          id="inline-end-input"
          type="password"
          placeholder="Enter password"
        />
        <InputGroupAddon align="inline-end">
          <Icon name="eye-off" />
        </InputGroupAddon>
      </InputGroup>
      <FieldDescription>Icon positioned at the end.</FieldDescription>
    </Field>
  );
}
