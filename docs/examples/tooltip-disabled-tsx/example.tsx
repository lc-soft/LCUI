import {
  Button,
  Text,
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@lcui/react";

export default function App() {
  return (
    <Tooltip>
      <TooltipTrigger>
        <Button variant="outline" disabled>
          Disabled
        </Button>
      </TooltipTrigger>
      <TooltipContent>
        <Text>This feature is currently unavailable</Text>
      </TooltipContent>
    </Tooltip>
  );
}
