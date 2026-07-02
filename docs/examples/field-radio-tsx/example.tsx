import {
  Field,
  FieldDescription,
  FieldLabel,
  FieldSet,
  RadioGroup,
  RadioGroupItem,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldSet>
        <FieldLabel>Subscription Plan</FieldLabel>
        <FieldDescription>
          Yearly and lifetime plans offer significant savings.
        </FieldDescription>
        <RadioGroup defaultValue="monthly">
          <Field orientation="horizontal">
            <RadioGroupItem value="monthly" id="plan-monthly" />
            <FieldLabel htmlFor="plan-monthly">
              Monthly ($9.99/month)
            </FieldLabel>
          </Field>
          <Field orientation="horizontal">
            <RadioGroupItem value="yearly" id="plan-yearly" />
            <FieldLabel htmlFor="plan-yearly">
              Yearly ($99.99/year)
            </FieldLabel>
          </Field>
          <Field orientation="horizontal">
            <RadioGroupItem value="lifetime" id="plan-lifetime" />
            <FieldLabel htmlFor="plan-lifetime">
              Lifetime ($299.99)
            </FieldLabel>
          </Field>
        </RadioGroup>
      </FieldSet>
    </Widget>
  );
}
