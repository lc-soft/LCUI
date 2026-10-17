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
  Select,
  SelectContent,
  SelectGroup,
  SelectItem,
  SelectTrigger,
  SelectValue,
  TextInput,
  Widget,
} from "@lcui/react";

const months = [
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "07",
  "08",
  "09",
  "10",
  "11",
  "12",
];

const years = ["2024", "2025", "2026", "2027", "2028", "2029"];

export default function App() {
  return (
    <Widget className="flex flex-col gap-5 max-w-[448px] p-1 -m-1">
      <FieldSet>
        <FieldLegend>Payment Method</FieldLegend>
        <FieldDescription>
          All transactions are secure and encrypted
        </FieldDescription>
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
              <Select>
                <SelectTrigger id="card-month">
                  <SelectValue placeholder="MM" />
                </SelectTrigger>
                <SelectContent>
                  <SelectGroup>
                    {months.map((item) => (
                      <SelectItem key={item} value={item}>
                        {item}
                      </SelectItem>
                    ))}
                  </SelectGroup>
                </SelectContent>
              </Select>
            </Field>
            <Field className="flex-1">
              <FieldLabel htmlFor="card-year">Year</FieldLabel>
              <Select>
                <SelectTrigger id="card-year">
                  <SelectValue placeholder="YYYY" />
                </SelectTrigger>
                <SelectContent>
                  <SelectGroup>
                    {years.map((item) => (
                      <SelectItem key={item} value={item}>
                        {item}
                      </SelectItem>
                    ))}
                  </SelectGroup>
                </SelectContent>
              </Select>
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
        <FieldDescription>
          The billing address associated with your payment method
        </FieldDescription>
        <FieldGroup>
          <Field orientation="horizontal">
            <Checkbox id="billing-same" />
            <FieldLabel htmlFor="billing-same" className="font-normal">
              Same as shipping address
            </FieldLabel>
          </Field>
        </FieldGroup>
      </FieldSet>
      <Field orientation="horizontal">
        <Button variant="primary">Submit</Button>
        <Button>Cancel</Button>
      </Field>
    </Widget>
  );
}
