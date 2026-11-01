import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
} from "@lcui/react";

export default function Example() {
  return (
    <Accordion
      className="w-[512px]"
      multiple
      defaultValue="notifications"
    >
      <AccordionItem value="notifications">
        <AccordionTrigger>Notification Settings</AccordionTrigger>
        <AccordionContent>
          Manage how you receive notifications. You can enable email alerts for
          updates or push notifications for mobile devices.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="privacy">
        <AccordionTrigger>Privacy & Security</AccordionTrigger>
        <AccordionContent>
          Control your privacy settings and security preferences. Enable
          two-factor authentication, manage connected devices, review active
          sessions, and configure data sharing preferences. You can also
          download your data or delete your account.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="billing">
        <AccordionTrigger>Billing & Subscription</AccordionTrigger>
        <AccordionContent>
          View your current plan, payment history, and upcoming invoices.
          Update your payment method, change your subscription tier, or cancel
          your subscription.
        </AccordionContent>
      </AccordionItem>
    </Accordion>
  );
}
