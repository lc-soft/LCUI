import {
  Button,
  ButtonGroup,
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
    <ButtonGroup>
      <Button variant="outline">Follow</Button>
      <DropdownMenu>
        <DropdownMenuTrigger>
          <Button variant="outline" size="icon">
            <Icon name="chevron-down" />
          </Button>
        </DropdownMenuTrigger>
        <DropdownMenuContent align="end" className="w-44">
          <DropdownMenuGroup>
            <DropdownMenuItem>
              <Icon name="speaker-off" />
              <Text>Mute Conversation</Text>
            </DropdownMenuItem>
            <DropdownMenuItem>
              <Icon name="checkmark" />
              <Text>Mark as Read</Text>
            </DropdownMenuItem>
            <DropdownMenuItem>
              <Icon name="warning" />
              <Text>Report Conversation</Text>
            </DropdownMenuItem>
            <DropdownMenuItem>
              <Icon name="person-delete" />
              <Text>Block User</Text>
            </DropdownMenuItem>
            <DropdownMenuItem>
              <Icon name="share" />
              <Text>Share Conversation</Text>
            </DropdownMenuItem>
            <DropdownMenuItem>
              <Icon name="copy" />
              <Text>Copy Conversation</Text>
            </DropdownMenuItem>
          </DropdownMenuGroup>
          <DropdownMenuSeparator />
          <DropdownMenuGroup>
            <DropdownMenuItem variant="destructive">
              <Icon name="delete" />
              <Text>Delete Conversation</Text>
            </DropdownMenuItem>
          </DropdownMenuGroup>
        </DropdownMenuContent>
      </DropdownMenu>
    </ButtonGroup>
  );
}
