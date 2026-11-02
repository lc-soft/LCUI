import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuTrigger,
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupButton,
  InputGroupInput,
  Text,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex w-full max-w-sm flex-col gap-4 clip-safe">
      <InputGroup>
        <InputGroupInput placeholder="Enter file name" />
        <InputGroupAddon align="inline-end">
          <DropdownMenu>
            <DropdownMenuTrigger>
              <InputGroupButton size="icon-xs">
                <Icon name="more-horizontal" />
              </InputGroupButton>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end">
              <DropdownMenuGroup>
                <DropdownMenuItem>
                  <Text>Settings</Text>
                </DropdownMenuItem>
                <DropdownMenuItem>
                  <Text>Copy path</Text>
                </DropdownMenuItem>
                <DropdownMenuItem>
                  <Text>Open location</Text>
                </DropdownMenuItem>
              </DropdownMenuGroup>
            </DropdownMenuContent>
          </DropdownMenu>
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Enter search query" />
        <InputGroupAddon align="inline-end">
          <DropdownMenu>
            <DropdownMenuTrigger>
              <InputGroupButton>
                <Text>Search In...</Text>
                <Icon name="chevron-down" />
              </InputGroupButton>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end">
              <DropdownMenuGroup>
                <DropdownMenuItem>
                  <Text>Documentation</Text>
                </DropdownMenuItem>
                <DropdownMenuItem>
                  <Text>Blog Posts</Text>
                </DropdownMenuItem>
                <DropdownMenuItem>
                  <Text>Changelog</Text>
                </DropdownMenuItem>
              </DropdownMenuGroup>
            </DropdownMenuContent>
          </DropdownMenu>
        </InputGroupAddon>
      </InputGroup>
    </Widget>
  );
}
