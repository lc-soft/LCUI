import {
  Button,
  ButtonGroup,
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuTrigger,
  Icon,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup className="w-[480px]">
      <ButtonGroup>
        <Button variant="outline" size="icon">
          <Icon name="arrow-left" />
        </Button>
      </ButtonGroup>
      <ButtonGroup>
        <Button variant="outline">Archive</Button>
        <Button variant="outline">Report</Button>
      </ButtonGroup>
      <ButtonGroup>
        <Button variant="outline">Snooze</Button>
        <DropdownMenu>
          <DropdownMenuTrigger>
            <Button variant="outline" size="icon">
              <Icon name="more-horizontal" />
            </Button>
          </DropdownMenuTrigger>
          <DropdownMenuContent>
            <DropdownMenuGroup>
              <DropdownMenuItem><Text>Mark as Read</Text></DropdownMenuItem>
              <DropdownMenuItem><Text>Archive</Text></DropdownMenuItem>
              <DropdownMenuItem><Text>Snooze</Text></DropdownMenuItem>
              <DropdownMenuItem><Text>Add to Calendar</Text></DropdownMenuItem>
              <DropdownMenuItem><Text>Add to List</Text></DropdownMenuItem>
            </DropdownMenuGroup>
          </DropdownMenuContent>
        </DropdownMenu>
      </ButtonGroup>
    </ButtonGroup>
  );
}
