import {
  Button,
  ButtonGroup,
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup className="w-[384px] clip-safe">
      <ButtonGroup>
        <Button variant="outline" size="icon">
          <Icon name="add" />
        </Button>
      </ButtonGroup>
      <ButtonGroup className="flex-1 min-w-0 clip-safe">
        <InputGroup>
          <InputGroupInput placeholder="Send a message..." />
          <InputGroupAddon align="inline-end">
            <Button variant="ghost" size="icon-xs" data-tooltip="Voice Mode">
              <Icon name="device-eq" />
            </Button>
          </InputGroupAddon>
        </InputGroup>
      </ButtonGroup>
    </ButtonGroup>
  );
}
