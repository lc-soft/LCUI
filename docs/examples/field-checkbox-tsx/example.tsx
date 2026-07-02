import {
  Checkbox,
  Field,
  FieldContent,
  FieldDescription,
  FieldGroup,
  FieldLabel,
  FieldLegend,
  FieldSeparator,
  FieldSet,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="flex flex-col gap-2 max-w-[400px]">
      <FieldSet>
        <FieldLegend>Show these items on the desktop</FieldLegend>
        <FieldDescription>Select the items you want to show on the desktop.</FieldDescription>
        <FieldGroup className="gap-3">
          <Field orientation="horizontal">
            <Checkbox id="finder-pref-9k2-hard-disks-ljj" defaultChecked />
            <FieldLabel htmlFor="finder-pref-9k2-hard-disks-ljj">
              Hard disks
            </FieldLabel>
          </Field>
          <Field orientation="horizontal">
            <Checkbox id="finder-pref-9k2-external-disks-1yg" />
            <FieldLabel htmlFor="finder-pref-9k2-external-disks-1yg">
              External disks
            </FieldLabel>
          </Field>
          <Field orientation="horizontal">
            <Checkbox id="finder-pref-9k2-cds-dvds-fzt" />
            <FieldLabel htmlFor="finder-pref-9k2-cds-dvds-fzt">
              CDs, DVDs, and iPods
            </FieldLabel>
          </Field>
          <Field orientation="horizontal">
            <Checkbox id="finder-pref-9k2-connected-servers-6l2" />
            <FieldLabel htmlFor="finder-pref-9k2-connected-servers-6l2">
              Connected servers
            </FieldLabel>
          </Field>
        </FieldGroup>
      </FieldSet>
      <FieldSeparator />
      <Field orientation="horizontal">
        <Checkbox id="finder-pref-9k2-sync-folders-nep" defaultChecked />
        <FieldContent>
          <FieldLabel htmlFor="finder-pref-9k2-sync-folders-nep">
            Sync Desktop &amp; Documents folders
          </FieldLabel>
          <FieldDescription>
            Your Desktop &amp; Documents folders are being synced with iCloud
            Drive. You can access them from other devices.
          </FieldDescription>
        </FieldContent>
      </Field>
    </Widget>
  );
}
