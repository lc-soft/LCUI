import {
  Attachment,
  AttachmentAction,
  AttachmentActions,
  AttachmentContent,
  AttachmentDescription,
  AttachmentMedia,
  AttachmentTitle,
  Icon,
  Spinner,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="mx-auto flex w-full max-w-sm flex-col gap-2">
      <Attachment state="idle">
        <AttachmentMedia>
          <Icon name="clock" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>selected-file.pdf</AttachmentTitle>
          <AttachmentDescription>Ready to upload</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Remove selected-file.pdf">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
      <Attachment state="uploading">
        <AttachmentMedia>
          <Spinner />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>design-system.zip</AttachmentTitle>
          <AttachmentDescription>Uploading · 64%</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Cancel upload">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
      <Attachment state="processing">
        <AttachmentMedia>
          <Icon name="document-text" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>market-research.pdf</AttachmentTitle>
          <AttachmentDescription>Processing document</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Remove market-research.pdf">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
      <Attachment state="error">
        <AttachmentMedia>
          <Icon name="document-error" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>financial-model.xlsx</AttachmentTitle>
          <AttachmentDescription>Upload failed. Try again.</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Retry upload">
            <Icon name="arrow-sync" />
          </AttachmentAction>
          <AttachmentAction aria-label="Remove financial-model.xlsx">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
      <Attachment state="done">
        <AttachmentMedia>
          <Icon name="checkmark" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>uploaded-report.pdf</AttachmentTitle>
          <AttachmentDescription>Uploaded · 1.8 MB</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Remove uploaded-report.pdf">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
    </Widget>
  );
}
