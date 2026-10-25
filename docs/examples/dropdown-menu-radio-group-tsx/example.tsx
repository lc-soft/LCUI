import {
  Button,
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuRadioGroup,
  DropdownMenuRadioItem,
  DropdownMenuTrigger,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <Button>Open</Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent className="w-32">
        <DropdownMenuLabel>Panel position</DropdownMenuLabel>
        <DropdownMenuRadioGroup value="bottom">
          <DropdownMenuRadioItem value="top">
            <Text>Top</Text>
          </DropdownMenuRadioItem>
          <DropdownMenuRadioItem value="bottom">
            <Text>Bottom</Text>
          </DropdownMenuRadioItem>
          <DropdownMenuRadioItem value="right">
            <Text>Right</Text>
          </DropdownMenuRadioItem>
        </DropdownMenuRadioGroup>
      </DropdownMenuContent>
    </DropdownMenu>
  );
}
