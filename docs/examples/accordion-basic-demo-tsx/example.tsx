import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
} from "@lcui/react";

export default function Example() {
  return (
    <Accordion className="w-[512px]" defaultValue="item-1">
      <AccordionItem value="item-1">
        <AccordionTrigger>How do I reset my password?</AccordionTrigger>
        <AccordionContent>
          Click on 'Forgot Password' on the login page, enter your email
          address, and we'll send you a link to reset your password. The link
          will expire in 24 hours.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="item-2">
        <AccordionTrigger>Can I change my subscription plan?</AccordionTrigger>
        <AccordionContent>
          Yes, you can upgrade or downgrade your plan at any time from your
          account settings. Changes will be reflected in your next billing
          cycle.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="item-3">
        <AccordionTrigger>What payment methods do you accept?</AccordionTrigger>
        <AccordionContent>
          We accept all major credit cards, PayPal, and bank transfers. All
          payments are processed securely through our payment partners.
        </AccordionContent>
      </AccordionItem>
    </Accordion>
  );
}
