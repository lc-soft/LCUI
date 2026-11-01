import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
  Button,
} from "@lcui/react";

export default function Example() {
  return (
    <Accordion className="w-[512px]" defaultValue="account">
      <AccordionItem value="account">
        <Button
          variant="ghost"
          size="sm"
          data-accordion-trigger="true"
        >
          Account settings
        </Button>
        <AccordionContent>
          Update your profile, email address, and password from the account
          settings panel.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="billing">
        <AccordionTrigger>Billing & invoices</AccordionTrigger>
        <AccordionContent>
          View invoices, update your payment method, or download receipts for
          your records.
        </AccordionContent>
      </AccordionItem>
    </Accordion>
  );
}
