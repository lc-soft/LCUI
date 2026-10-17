import {
  Checkbox,
  Field,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldLegend,
  FieldSet,
} from "@lcui/react";

export default function App() {
  return (
    <FieldSet className="max-w-sm">
      <FieldLegend>Show these items on the desktop:</FieldLegend>
      <FieldDescription>
        Select the items you want to show on the desktop.
      </FieldDescription>
      <FieldGroup className="gap-3">
        <Field orientation="horizontal">
          <Checkbox id="finder-pref-hard-disks" checked />
          <FieldLabel for="finder-pref-hard-disks" className="font-normal">
            Hard disks
          </FieldLabel>
        </Field>
        <Field orientation="horizontal">
          <Checkbox id="finder-pref-external-disks" checked />
          <FieldLabel for="finder-pref-external-disks" className="font-normal">
            External disks
          </FieldLabel>
        </Field>
        <Field orientation="horizontal">
          <Checkbox id="finder-pref-cds-dvds" />
          <FieldLabel for="finder-pref-cds-dvds" className="font-normal">
            CDs, DVDs, and iPods
          </FieldLabel>
        </Field>
        <Field orientation="horizontal">
          <Checkbox id="finder-pref-servers" />
          <FieldLabel for="finder-pref-servers" className="font-normal">
            Connected servers
          </FieldLabel>
        </Field>
      </FieldGroup>
    </FieldSet>
  );
}
