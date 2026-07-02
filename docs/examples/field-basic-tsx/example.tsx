import {
  Button,
  Checkbox,
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldLegend,
  FieldSeparator,
  FieldSet,
  TextInput,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldSet>
        <FieldLegend>Payment Method</FieldLegend>
        <FieldDescription>All transactions are secure and encrypted</FieldDescription>
        <FieldGroup>
          <Field>
            <FieldLabel htmlFor="name-on-card">Name on Card</FieldLabel>
            <TextInput id="name-on-card" placeholder="Evil Rabbit" />
          </Field>
          <Field>
            <FieldLabel htmlFor="card-number">Card Number</FieldLabel>
            <TextInput id="card-number" placeholder="1234 5678 9012 3456" />
            <FieldDescription>Enter your 16-digit card number</FieldDescription>
          </Field>
          <div className="flex gap-4">
            <Field className="flex-1">
              <FieldLabel htmlFor="card-month">Month</FieldLabel>
              <TextInput id="card-month" placeholder="MM" />
            </Field>
            <Field className="flex-1">
              <FieldLabel htmlFor="card-year">Year</FieldLabel>
              <TextInput id="card-year" placeholder="YYYY" />
            </Field>
            <Field className="flex-1">
              <FieldLabel htmlFor="card-cvv">CVV</FieldLabel>
              <TextInput id="card-cvv" placeholder="123" />
            </Field>
          </div>
        </FieldGroup>
      </FieldSet>
      <FieldSeparator />
      <FieldSet>
        <FieldLegend>Billing Address</FieldLegend>
        <FieldDescription>The billing address associated with your payment method</FieldDescription>
        <FieldGroup>
          <Field orientation="horizontal">
            <Checkbox id="billing-same" />
            <FieldLabel htmlFor="billing-same">Same as shipping address</FieldLabel>
          </Field>
        </FieldGroup>
      </FieldSet>
      <Field orientation="horizontal">
        <Button>Submit</Button>
        <Button>Cancel</Button>
      </Field>
    </Widget>
  );
}
