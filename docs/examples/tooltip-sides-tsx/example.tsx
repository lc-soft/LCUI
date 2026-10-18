import {
  Button,
  Text,
  Tooltip,
  TooltipContent,
  TooltipTrigger,
  Widget,
} from "@lcui/react";

const sides = ["left", "top", "bottom", "right"] as const;

export default function App() {
  return (
    <Widget className="tooltip-sides-demo">
      {sides.map((side) => (
        <Tooltip key={side}>
          <TooltipTrigger>
            <Button variant="outline" className="capitalize">
              {side}
            </Button>
          </TooltipTrigger>
          <TooltipContent side={side}>
            <Text>Add to library</Text>
          </TooltipContent>
        </Tooltip>
      ))}
    </Widget>
  );
}
