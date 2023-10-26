from icecream import ic
from typing import List

from NTFS.boot_sector import BootSector
from NTFS.mft_file import MFTFile
from NTFS.mft_entry import MFTEntry
# for more beautiful traceback
from UI.utils import GLOBAL_CONSOLE

class NTFS:
    def __init__(self, vol_name: str) -> None:
        self.name = vol_name

        # boot sector
        self.boot_sector = BootSector(self.name)

        # first $MFT file
        self.mft_file = MFTFile(self.boot_sector)

        # entry list
        self.entry_list: List[MFTEntry] = []

        # add all remaining entry to entry list
        sector_per_entry = int(self.boot_sector.MFT_entry_size / self.boot_sector.bytes_per_sector)
        entry_count = 2

        for _ in range(sector_per_entry, self.mft_file.num_sector_mft, sector_per_entry):
            data = self.boot_sector.f.read(self.boot_sector.MFT_entry_size)

            if data[:4] == b"FILE":
                # Because entry 13->16 reserved for $Extend extension
                if entry_count >= 13 and entry_count <= 16:
                    entry_count += 1
                    continue

                entry = MFTEntry(data)
                if entry.flag == 0 or entry.flag == 2:
                    del entry
                else:
                    self.entry_list.append(entry)

                entry_count += 1


    @staticmethod
    def check_ntfs(vol_name: str) -> bool:
        try:
            with open(r'\\.\%s' % vol_name, "rb") as f:
                name = f.read(512)[0x03 : 0x03 + 8].decode()
                if name == "NTFS    ":
                    return True
                else:
                    return False
        except FileNotFoundError:
            GLOBAL_CONSOLE.print_exception(word_wrap=True)
            exit()
        except PermissionError:
            GLOBAL_CONSOLE.print_exception(word_wrap=True)
            exit()
        except Exception:
            GLOBAL_CONSOLE.print_exception(word_wrap=True)
            exit()