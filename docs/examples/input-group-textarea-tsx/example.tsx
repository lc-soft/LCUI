import {
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupButton,
  InputGroupText,
  InputGroupTextarea,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <InputGroup className="w-full max-w-md">
      <InputGroupTextarea
        id="textarea-code-32"
        className="min-h-[200px] font-mono"
        placeholder="console.log('Hello, world!');"
      />
      <InputGroupAddon align="block-start">
        <InputGroupText className="font-mono font-medium">
          <Icon name="code" />
          script.js
        </InputGroupText>
        <InputGroupButton size="icon-xs" className="ml-auto">
          <Icon name="copy" />
        </InputGroupButton>
      </InputGroupAddon>
      <InputGroupAddon align="block-end">
        <InputGroupText>Line 1, Column 1</InputGroupText>
        <InputGroupButton
          variant="primary"
          size="sm"
          className="ml-auto"
        >
          <Text>Run</Text>
          <Icon name="arrow-enter-left" />
        </InputGroupButton>
      </InputGroupAddon>
    </InputGroup>
  );
}
