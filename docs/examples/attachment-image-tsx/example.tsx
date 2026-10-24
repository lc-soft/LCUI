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
    <Widget className="mx-auto w-full max-w-sm">
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
            <AttachmentActions>
              <AttachmentAction aria-label={`Remove ${image.name}`}>
                <Icon name="dismiss" />
              </AttachmentAction>
            </AttachmentActions>
          </Attachment>
        ))}
      </AttachmentGroup>
    </Widget>
  );
}
