package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.dto.OwnerDTO;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class OwnerMapperTest {
    @Mock
    private PropertyService propertyService;
    private OwnerDTOConverter ownerDTOConverter;

    @BeforeEach
    public void setUp() {
        propertyService = mock(PropertyService.class);
    }

    @Test
    public void testOwnerToOwnerDTO() {
        Owner owner = new Owner("testOwmer@mail.com", null, List.of());
        OwnerDTO expectedOwnerDTO = new OwnerDTO("testOwmer@mail.com", null, List.of());
        ownerDTOConverter = new OwnerDTOConverter(propertyService);

        OwnerDTO actualOwnerDTO = ownerDTOConverter.toDTO(owner);
        assertNotNull(actualOwnerDTO);
        assertEquals(expectedOwnerDTO.getEmail(), actualOwnerDTO.getEmail());
        assertEquals(expectedOwnerDTO.getPhoneNumber(), actualOwnerDTO.getPhoneNumber());
        assertEquals(expectedOwnerDTO.getOwnedPropertiesId(), actualOwnerDTO.getOwnedPropertiesId());
    }

    @Test
    public void testOwnerDTOToOwner() {
        OwnerDTO ownerDTO = new OwnerDTO("testOwmer@mail.com", null, List.of());
        Owner expectedOwner = new Owner("testOwmer@mail.com", null, List.of());
        ownerDTOConverter = new OwnerDTOConverter(propertyService);
        when(propertyService.getEntitiesByIds(ownerDTO.getOwnedPropertiesId()))
                .thenReturn(expectedOwner.getOwnedProperties());

        Owner actualOwner = ownerDTOConverter.toEntity(ownerDTO);
        assertNotNull(actualOwner);
        assertEquals(expectedOwner.getEmail(), actualOwner.getEmail());
        assertEquals(expectedOwner.getPhoneNumber(), actualOwner.getPhoneNumber());
        assertEquals(expectedOwner.getOwnedProperties(), actualOwner.getOwnedProperties());

    }
}
