import {
  Button,
  Field,
  FieldLabel,
  Popover,
  PopoverContent,
  PopoverDescription,
  PopoverHeader,
  PopoverTitle,
  PopoverTrigger,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Popover>
      <PopoverTrigger>
        <Button variant="outline">Open popover</Button>
      </PopoverTrigger>
      <PopoverContent className="w-80">
        <PopoverHeader>
          <PopoverTitle>Dimensions</PopoverTitle>
          <PopoverDescription>
            Set the dimensions for the layer.
          </PopoverDescription>
        </PopoverHeader>
        <Widget className="popover-form-grid clip-safe">
          <Field orientation="horizontal">
            <FieldLabel htmlFor="popover-width">Width</FieldLabel>
            <TextInput
              id="popover-width"
              className="popover-form-input"
              defaultValue="100%"
            />
          </Field>
          <Field orientation="horizontal">
            <FieldLabel htmlFor="popover-max-width">Max. width</FieldLabel>
            <TextInput
              id="popover-max-width"
              className="popover-form-input"
              defaultValue="300px"
            />
          </Field>
          <Field orientation="horizontal">
            <FieldLabel htmlFor="popover-height">Height</FieldLabel>
            <TextInput
              id="popover-height"
              className="popover-form-input"
              defaultValue="25px"
            />
          </Field>
          <Field orientation="horizontal">
            <FieldLabel htmlFor="popover-max-height">Max. height</FieldLabel>
            <TextInput
              id="popover-max-height"
              className="popover-form-input"
              defaultValue="none"
            />
          </Field>
        </Widget>
      </PopoverContent>
    </Popover>
  );
}
