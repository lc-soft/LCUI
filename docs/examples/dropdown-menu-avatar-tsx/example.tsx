import {
  Button,
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
  Icon,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <Button className="h-10 w-10 rounded-full p-0">LR</Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent align="end">
        <DropdownMenuGroup>
          <DropdownMenuItem>
            <Icon name="checkmark-circle" />
            <Text>Account</Text>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="payment" />
            <Text>Billing</Text>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="alert" />
            <Text>Notifications</Text>
          </DropdownMenuItem>
        </DropdownMenuGroup>
        <DropdownMenuSeparator />
        <DropdownMenuItem>
          <Icon name="sign-out" />
          <Text>Sign Out</Text>
        </DropdownMenuItem>
      </DropdownMenuContent>
    </DropdownMenu>
  );
}
