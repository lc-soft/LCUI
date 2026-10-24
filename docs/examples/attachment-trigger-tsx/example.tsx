import {
  Attachment,
  AttachmentAction,
  AttachmentActions,
  AttachmentContent,
  AttachmentDescription,
  AttachmentMedia,
  AttachmentTitle,
  AttachmentTrigger,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogOverlay,
  DialogTitle,
  Icon,
  Portal,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="mx-auto w-full max-w-sm p-1">
      <Attachment>
        <AttachmentMedia>
          <Icon name="document" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>research-summary.pdf</AttachmentTitle>
          <AttachmentDescription>Open preview dialog</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Copy link">
            <Icon name="copy" />
          </AttachmentAction>
          <AttachmentAction aria-label="Remove research-summary.pdf">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
        <AttachmentTrigger
          data-dialog-trigger="attachment-preview"
          aria-label="Preview research-summary.pdf"
        />
      </Attachment>
      <Portal>
        <DialogOverlay data-dialog-target="attachment-preview">
          <DialogContent>
            <DialogHeader>
              <DialogTitle>research-summary.pdf</DialogTitle>
              <DialogDescription>
                The attachment trigger opens this dialog while the actions stay
                independently clickable.
              </DialogDescription>
            </DialogHeader>
            <button data-dialog-close="attachment-preview">Close</button>
          </DialogContent>
        </DialogOverlay>
      </Portal>
    </Widget>
  );
}
