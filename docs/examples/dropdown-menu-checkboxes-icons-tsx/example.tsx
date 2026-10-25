import {
  Button,
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuLabel,
  DropdownMenuTrigger,
  Icon,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <Button>Notifications</Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent className="w-56">
        <DropdownMenuGroup>
          <DropdownMenuLabel>Notification Preferences</DropdownMenuLabel>
          <DropdownMenuCheckboxItem checked>
            <Icon name="mail" />
            <Text>Email notifications</Text>
          </DropdownMenuCheckboxItem>
          <DropdownMenuCheckboxItem>
            <Icon name="chat" />
            <Text>SMS notifications</Text>
          </DropdownMenuCheckboxItem>
          <DropdownMenuCheckboxItem checked>
            <Icon name="alert" />
            <Text>Push notifications</Text>
          </DropdownMenuCheckboxItem>
        </DropdownMenuGroup>
      </DropdownMenuContent>
    </DropdownMenu>
  );
}
