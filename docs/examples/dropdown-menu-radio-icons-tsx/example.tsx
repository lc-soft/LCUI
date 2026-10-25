import {
  Button,
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuLabel,
  DropdownMenuRadioGroup,
  DropdownMenuRadioItem,
  DropdownMenuTrigger,
  Icon,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <Button>Payment Method</Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent className="w-56">
        <DropdownMenuGroup>
          <DropdownMenuLabel>Select Payment Method</DropdownMenuLabel>
          <DropdownMenuRadioGroup value="card">
            <DropdownMenuRadioItem value="card">
              <Icon name="payment" />
              <Text>Credit Card</Text>
            </DropdownMenuRadioItem>
            <DropdownMenuRadioItem value="paypal">
              <Icon name="wallet" />
              <Text>PayPal</Text>
            </DropdownMenuRadioItem>
            <DropdownMenuRadioItem value="bank">
              <Icon name="building-bank" />
              <Text>Bank Transfer</Text>
            </DropdownMenuRadioItem>
          </DropdownMenuRadioGroup>
        </DropdownMenuGroup>
      </DropdownMenuContent>
    </DropdownMenu>
  );
}
