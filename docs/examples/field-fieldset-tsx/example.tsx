import {
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldLegend,
  FieldSet,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldSet>
        <FieldLegend>Address Information</FieldLegend>
        <FieldDescription>We need your address to deliver your order.</FieldDescription>
        <FieldGroup>
          <Field>
            <FieldLabel htmlFor="street">Street Address</FieldLabel>
            <TextInput id="street" placeholder="123 Main St" />
          </Field>
          <Field>
            <FieldLabel htmlFor="city">City</FieldLabel>
            <TextInput id="city" placeholder="New York" />
          </Field>
          <Field>
            <FieldLabel htmlFor="postal-code">Postal Code</FieldLabel>
            <TextInput id="postal-code" placeholder="90502" />
          </Field>
        </FieldGroup>
      </FieldSet>
    </Widget>
  );
}
