import * as React from 'react';
import Button from '@mui/material/Button';
import Menu from '@mui/material/Menu';
import MenuItem from '@mui/material/MenuItem';
import SettingsIcon from '@mui/icons-material/Settings';
import CheckIcon from '@mui/icons-material/Check';
interface Props {
  onChange: (mode: string) => void,
  currentMode: string
}

export default function TransportMode({onChange, currentMode}: Props) {
  const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null);
  const open = Boolean(anchorEl);
  const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
    setAnchorEl(event.currentTarget);
  };
  const handleClose = () => {
    setAnchorEl(null);
  };

  return (
    <div style={{flex: 1, textAlign: "right"}}>
      <Button
        id="basic-button"
        aria-controls={open ? 'basic-menu' : undefined}
        aria-haspopup="true"
        aria-expanded={open ? 'true' : undefined}
        onClick={handleClick}
      >
        <SettingsIcon />
      </Button>
      <Menu
        id="basic-menu"
        anchorEl={anchorEl}
        open={open}
        onClose={handleClose}
        MenuListProps={{
          'aria-labelledby': 'basic-button',
        }}
      >
        <MenuItem disabled>Transport Mode</MenuItem>
        <MenuItem onClick={() => {onChange("rpc"); handleClose();}}>
          {<CheckIcon style={{opacity: currentMode === "rpc" ? 1 : 0}} />} RPC+HDLC
        </MenuItem>
        <MenuItem onClick={() => {onChange("stream"); handleClose();}}>
          {<CheckIcon style={{opacity: currentMode === "stream" ? 1 : 0}} />} Streaming Serial+Detokenizer
        </MenuItem>
        <MenuItem onClick={() => {onChange("raw"); handleClose();}}>
          {<CheckIcon style={{opacity: currentMode === "raw" ? 1 : 0}} />} Raw Serial Output
        </MenuItem>
      </Menu>
    </div>
  );
}
