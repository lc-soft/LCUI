import {
  Attachment,
  AttachmentAction,
  AttachmentActions,
  AttachmentContent,
  AttachmentDescription,
  AttachmentGroup,
  AttachmentMedia,
  AttachmentTitle,
  Icon,
  Spinner,
  Widget,
} from "@lcui/react";

import deskReference from "../assets/attachment/desk-reference.jpg";
import officeReference from "../assets/attachment/office-reference.jpg";
import workspace from "../assets/attachment/workspace.jpg";

const images = [
  {
    name: "workspace.png",
    meta: "PNG · 820 KB",
    src: workspace,
    alt: "Workspace",
  },
  {
    name: "desk-reference.jpg",
    meta: "JPG · 1.1 MB",
    src: deskReference,
    alt: "Desk",
  },
  {
    name: "office-reference.jpg",
    meta: "JPG · 940 KB",
    src: officeReference,
    alt: "Office",
  },
];

export default function Example() {
  return (
    <Widget className="mx-auto flex w-full max-w-sm flex-col gap-3 py-6">
      <AttachmentGroup>
        {images.map((image) => (
          <Attachment key={image.name} orientation="vertical">
            <AttachmentMedia
              variant="image"
              aria-label={image.alt}
              style={{ backgroundImage: `url(${image.src})` }}
            />
            <AttachmentContent>
              <AttachmentTitle>{image.name}</AttachmentTitle>
              <AttachmentDescription>{image.meta}</AttachmentDescription>
            </AttachmentContent>
          </Attachment>
        ))}
      </AttachmentGroup>
      <Attachment state="uploading">
        <AttachmentMedia>
          <Spinner />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>sales-dashboard.pdf</AttachmentTitle>
          <AttachmentDescription>Uploading · 64%</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Cancel upload">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
      <Attachment>
        <AttachmentMedia>
          <Icon name="code" />
        </AttachmentMedia>
        <AttachmentContent>
          <AttachmentTitle>message-renderer.tsx</AttachmentTitle>
          <AttachmentDescription>TypeScript · 12 KB</AttachmentDescription>
        </AttachmentContent>
        <AttachmentActions>
          <AttachmentAction aria-label="Remove message-renderer.tsx">
            <Icon name="dismiss" />
          </AttachmentAction>
        </AttachmentActions>
      </Attachment>
    </Widget>
  );
}
