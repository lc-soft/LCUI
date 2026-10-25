import {
  Button,
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  Field,
  FieldLabel,
  Icon,
  Text,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="w-[320px]">
      <Card size="sm" className="w-full">
        <CardHeader>
          <CardTitle>Radius</CardTitle>
          <CardDescription>Set the corner radius of the element.</CardDescription>
        </CardHeader>
        <CardContent>
          <Collapsible>
            <CollapsibleTrigger>
              <Button
                variant="ghost"
                className="w-full justify-between border-0 bg-transparent"
              >
                <Text>Advanced settings</Text>
                <Icon name="chevron-down" />
              </Button>
            </CollapsibleTrigger>
            <CollapsibleContent>
              <Widget className="flex gap-2 pt-2.5">
                <Field className="flex w-1/2 flex-col gap-1">
                  <FieldLabel htmlFor="radius-x">Radius X</FieldLabel>
                  <TextInput id="radius-x" placeholder="0" />
                </Field>
                <Field className="flex w-1/2 flex-col gap-1">
                  <FieldLabel htmlFor="radius-y">Radius Y</FieldLabel>
                  <TextInput id="radius-y" placeholder="0" />
                </Field>
              </Widget>
            </CollapsibleContent>
          </Collapsible>
        </CardContent>
      </Card>
    </Widget>
  );
}
