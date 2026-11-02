import {
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex w-full max-w-sm flex-col gap-6 clip-safe">
      <InputGroup>
        <InputGroupInput placeholder="Search..." />
        <InputGroupAddon>
          <Icon name="search" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput type="email" placeholder="Enter your email" />
        <InputGroupAddon>
          <Icon name="mail" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Card number" />
        <InputGroupAddon>
          <Icon name="payment" />
        </InputGroupAddon>
        <InputGroupAddon align="inline-end">
          <Icon name="checkmark" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Card number" />
        <InputGroupAddon align="inline-end">
          <Icon name="star" />
          <Icon name="info" />
        </InputGroupAddon>
      </InputGroup>
    </Widget>
  );
}
