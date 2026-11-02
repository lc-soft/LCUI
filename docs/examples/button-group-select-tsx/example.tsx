import {
  Button,
  ButtonGroup,
  Icon,
  Select,
  SelectContent,
  SelectGroup,
  SelectItem,
  SelectTrigger,
  SelectValue,
  TextInput,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup className="w-[384px]">
      <ButtonGroup className="flex-1 min-w-0 clip-safe">
        <Select defaultValue="$">
          <SelectTrigger className="font-mono">
            <SelectValue placeholder="$" />
          </SelectTrigger>
          <SelectContent>
            <SelectGroup>
              <SelectItem value="$">$ US Dollar</SelectItem>
              <SelectItem value="EUR">EUR Euro</SelectItem>
              <SelectItem value="GBP">GBP British Pound</SelectItem>
            </SelectGroup>
          </SelectContent>
        </Select>
        <TextInput placeholder="10.00" />
      </ButtonGroup>
      <ButtonGroup className="clip-safe">
        <Button variant="outline" size="icon">
          <Icon name="arrow-right" />
        </Button>
      </ButtonGroup>
    </ButtonGroup>
  );
}
