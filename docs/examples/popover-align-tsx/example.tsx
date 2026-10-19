import {
  Button,
  Popover,
  PopoverContent,
  PopoverDescription,
  PopoverHeader,
  PopoverTitle,
  PopoverTrigger,
  Widget,
} from "@lcui/react";

function AlignedPopover({ align, label }: { align: "start" | "center" | "end"; label: string }) {
  return (
    <Popover>
      <PopoverTrigger>
        <Button variant="outline">{label}</Button>
      </PopoverTrigger>
      <PopoverContent align={align}>
        <PopoverHeader>
          <PopoverTitle>{label}</PopoverTitle>
          <PopoverDescription>
            This popover uses a different alignment.
          </PopoverDescription>
        </PopoverHeader>
      </PopoverContent>
    </Popover>
  );
}

export default function App() {
  return (
    <Widget className="popover-align-demo">
      <AlignedPopover align="start" label="Start" />
      <AlignedPopover align="center" label="Center" />
      <AlignedPopover align="end" label="End" />
    </Widget>
  );
}
