import {
  Button,
  ButtonGroup,
  Field,
  FieldDescription,
  FieldLabel,
  Icon,
  Popover,
  PopoverContent,
  PopoverDescription,
  PopoverHeader,
  PopoverTitle,
  PopoverTrigger,
  TextInput,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup>
      <Button variant="outline">
        <Icon name="bot" />
        Copilot
      </Button>
      <Popover>
        <PopoverTrigger>
          <Button variant="outline" size="icon" aria-label="Open Popover">
            <Icon name="chevron-down" />
          </Button>
        </PopoverTrigger>
        <PopoverContent align="end" className="rounded-xl text-sm">
          <PopoverHeader>
            <PopoverTitle>Start a new task with Copilot</PopoverTitle>
            <PopoverDescription>
              Describe your task in natural language.
            </PopoverDescription>
          </PopoverHeader>
          <Field>
            <FieldLabel htmlFor="task" className="sr-only">
              Task Description
            </FieldLabel>
            <TextInput
              id="task"
              placeholder="I need to..."
              className="min-h-16 resize-none"
            />
            <FieldDescription>
              Copilot will open a pull request for review.
            </FieldDescription>
          </Field>
        </PopoverContent>
      </Popover>
    </ButtonGroup>
  );
}
