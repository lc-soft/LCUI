import {
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupButton,
  InputGroupInput,
  InputGroupText,
  InputGroupTextarea,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <FieldGroup className="w-[384px]">
      <Field>
        <FieldLabel htmlFor="block-start-input">Input</FieldLabel>
        <InputGroup>
          <InputGroupInput
            id="block-start-input"
            placeholder="Enter your name"
          />
          <InputGroupAddon align="block-start">
            <InputGroupText>Full Name</InputGroupText>
          </InputGroupAddon>
        </InputGroup>
        <FieldDescription>Header positioned above the input.</FieldDescription>
      </Field>
      <Field>
        <FieldLabel htmlFor="block-start-textarea">Textarea</FieldLabel>
        <InputGroup>
          <InputGroupTextarea
            id="block-start-textarea"
            className="font-mono"
            placeholder="console.log('Hello, world!');"
          />
          <InputGroupAddon align="block-start">
            <Icon name="code" />
            <InputGroupText className="font-mono">
              <Text>script.js</Text>
            </InputGroupText>
            <InputGroupButton size="icon-xs" className="ml-auto">
              <Icon name="copy" />
            </InputGroupButton>
          </InputGroupAddon>
        </InputGroup>
        <FieldDescription>
          Header positioned above the textarea.
        </FieldDescription>
      </Field>
    </FieldGroup>
  );
}
